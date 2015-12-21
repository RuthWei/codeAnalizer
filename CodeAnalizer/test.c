    char name[NAME_SIZE]={'\0'};
    sprintf(name, "./file/%s.ptr", (fs->_file).file_name);		//临时文件处理 为.ptr文件表示记录的是读写指针 ptr
    sprintf(filename,"./file/%s",(fs->_file).file_name);
    int exist_ptr = 0;
    int exist_des = 0;
    
    exist_ptr = access(name, F_OK);
    exist_des = access(filename, F_OK);
    
    if(exist_ptr == 0 && exist_des == 0){				//判断目标文件和记录断点文件是否存在，必须都存在才可断点续传
        conect_server(&scktid, &addr, addr_len);		//创建socket并链接服务器
        
        write_cmd(fs, download, 9);						//表示使用断点续传
        send(scktid, fs, sizeof(file_package), 0);		//发送下载请求
        memset(fs, 0, sizeof(file_package));
        recv(scktid, fs, sizeof(file_package), MSG_WAITALL);		//接收服务器命令
        
        if(fs->cmd == download && fs->ack == 1){		//请求成功
            if((fp_ptr = fopen(name, "r")) == NULL){
                perror("Open ptr file ERROR");
                close(scktid);
                return 2;
            }
            fread(&(fs->_file), sizeof((fs->_file)), 1, fp_ptr);	//用记录的信息初始化文件信息结构体
            fclose(fp_ptr);
            printf("Will use the breakpoint translate file\n");
            close(scktid);
            return 1;
        }else if(fs->cmd == download && fs->ack == 3){
            close(scktid);
            return 2;										//请求下载失败
        }
    }else{
        conect_server(&scktid, &addr, addr_len);
        
        write_cmd(fs, download, 8);							//表示请求无断点下载
        printf("Please input thread num,max is %d\n",THREAD_NUM);
        scanf("%d",&((fs->_file).thread_sum));
        getchar();
        send(scktid, fs, sizeof(file_package), 0);			//发送下载请求
        memset(fs, 0, sizeof(file_package));
        recv(scktid, fs, sizeof(file_package), MSG_WAITALL);//等待接受服务器命令
        
        if(fs->cmd == download && fs->ack == 2){			//请求成功创建相关文件
            if((fp_des = fopen(filename, "w+")) == NULL){
                perror("Make new destination file ERROR");
                close(scktid);
                return 2;
            }
            fclose(fp_des);
            if((fp_ptr = fopen(name, "w+")) == NULL){
                perror("Make new ptr file ERROR");
                return 2;
            }
            fclose(fp_ptr);
            close(scktid);
            return 1;
        }else if(fs->cmd == download && fs->ack == 3){
            close(scktid);
            return 2;										//请求下载失败
        }
    }
    return -1;
}

void _download()
{
    int _init = 0;
    file_package file;
    memset(&file, 0, sizeof(file));
    printf("\nPlease input src file|Press Enter cancel\n");
    gets(file._file.file_name);
    if(strlen(file._file.file_name)==0)
        return;
    _init = download_init(&file);						//向服务器请求下载，并接受请求结果
    
    pthread_t th_cpy[THREAD_NUM];
    int pt_count = 0;
    switch(_init)
    {
        case 1:
            pt_count = 0;
            while(pt_count < (file._file.thread_sum))	//根据线程数创建线程
            {
                sem_wait(&sem1);
                file._file.th_num = pt_count;
                printf("th_num is %d\n",file._file.th_num);
                printf("block size is %d\n",file._file.block_size);
                printf("tail size is %d\n",file._file.tail_size);
                printf("size is %d\n", file._file.size);
                printf("tcpsize is %d\n",file._file.tcpy_size[pt_count]);//文件相关的提示信息
                sem_post(&sem2);
                printf("Creating thread %d...\n",file._file.th_num);
                if(0 != pthread_create(&th_cpy[pt_count], NULL, thr_cpy, (void *)&file)){
                    perror("Creating thread ERROR");
                    exit(0);
                }
                printf("Creating thread:Success!\n");
                pt_count++;
            }
            download_show(&file);
            sleep(1);
            for(pt_count = 0; pt_count< (file._file.thread_sum); pt_count++)		//等待所有线程退出
                pthread_join(th_cpy[pt_count],NULL);
            break;
        case 2:
            printf("\t\t\033[0;31m<<Not found src file>>\033[0m\n");
            sleep(1);
            return;
        default:
            printf("\t\t\033[0;31m<<Unknow Error>>\033[0m\n");
            sleep(1);
            return;
    }
}

void *thr_cpy(void *pfile) 									//复制文件子线程
{
    file_package *pf = (file_package*)pfile;
    file_package th_file;
    
    memset(&th_file, 0, sizeof(th_file));
    sem_wait(&sem2);
    memcpy(&th_file, pf, sizeof(file_package));
    sem_post(&sem1);
    char filename[NAME_SIZE] ={'\0'};
    sprintf(filename, "./file/%s", (th_file._file).file_name);
    int n_size = (th_file._file).th_num;
    
    FILE *fp_des = NULL;
    if((fp_des = fopen(filename, "r+")) == NULL){
        perror("Open destination file ERROR");
        pthread_exit(NULL);
    }
    
    int rw_ptr = ((th_file._file).block_size)*n_size + (((th_file._file).tcpy_size)[n_size]);//计算移动指针的位置
    printf("%d ptr is %d\n",n_size,rw_ptr);
    fseek(fp_des, rw_ptr, SEEK_SET);
    
    int scktid = 0;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    
    write_cmd(&th_file, download, 7);					//表示准备好接受数据
    conect_server(&scktid, &addr, addr_len);			//创建socket并链接服务器
    send(scktid, &th_file, sizeof(th_file), 0);			//发送下载准备好命令
    
    while(1)
    {
        memset(&th_file, 0, sizeof(th_file));
        recv(scktid, &th_file, sizeof(th_file), MSG_WAITALL);
        if(((th_file.cmd) == download) && ((th_file.ack) == 4)){
            fwrite(th_file.buf, 1, th_file.ret, fp_des);
            pthread_mutex_lock(&mutex);
            (pf->_file).tcpy_size[n_size] += (th_file.ret);
            pthread_mutex_unlock(&mutex);
        }else if(((th_file.cmd) == download) && ((th_file.ack) == 5)){
            printf("%d part success\n",n_size+1);
            break;
        }
    }
    fclose(fp_des);
    close(scktid);
    pthread_exit(NULL);	
}

void download_show(file_package *pf) 			//显示进度 以及记录断点子线程
{
    int dld_size = 0;
    int pet = 0;
    short i;
    char name[64];
    memset(name, 0, sizeof(name));				
    sprintf(name, "./file/%s.ptr", (pf->_file).file_name);
    
    FILE *fp_ptr = NULL;
    int size = (pf->_file).size;
    int th_sum = (pf->_file).thread_sum;
    /*
    while(dld_size < ((pf->_file).size))
    {
        dld_size = 0;
        pthread_mutex_lock(&mutex);
        for (i = 0; i < (pf->_file).thread_sum; i++)
            dld_size += (pf->_file).tcpy_size[i];
        pthread_mutex_unlock(&mutex);
        
        pet = (int)(((float)(dld_size)/(float)size*100));	//计算已下载的百分比
      /*
        fp_ptr = fopen(name,"r+");
        fwrite(&(pf->_file), sizeof(file_info), 1, fp_ptr);  //记录实时读写位置
        fclose(fp_ptr);
        //printf("Thread sum is %d\n",th_sum);
        printf("Copy has finished %d%%\n",pet);
        printf("Finshed %d Bytes\n",dld_size);				//显示相关信息			
        sleep(1);											//进度显示刷新以及断点记录频率为1s
    
    }
    */
    printf("Download Success\n");
    remove(name);											//删除临时文件
}

//把一个数组中元素加2后再赋给这个数组
void arrayAdd2()
{
    int len = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < len; i++) {
        arr[i] += 2;
    }
}
