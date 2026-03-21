#include "kernel/types.h"   // Định nghĩa các kiểu dữ liệu cơ bản
#include "kernel/stat.h"    // Định nghĩa struct stat (thông tin file)
#include "user/user.h"      // Định nghĩa các system call dùng được trong user space
#include "kernel/param.h"

// while(còn dòng trong stdin) {
//     // 1. Đọc 1 dòng từ stdin
//     // 2. Tách dòng thành các từ
//     // 3. Ghép argv gốc + các từ vừa đọc
//     // 4. fork() → tạo tiến trình con
//     //    exec() → chạy lệnh trong tiến trình con
//     // 5. wait() → chờ con chạy xong
//     // 6. Lặp lại
// }

int main(int argc, char *argv[])
{
    // Kiểm tra tham số
    if(argc < 2){
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    char *new_argv[MAXARG];
    for(int i = 1; i < argc; i++)
    {
        new_argv[i - 1] = argv[i];
    }

    char buf[512];
    int n = 0;
    char c;
    while(read(0, &c, 1) > 0)
    {
        if(c == '\n')
        {
            buf[n] = '\0';
            int j = argc - 1;
            int start = 0;
            for(int k = 0; k <= n; k++)
            {
                if(buf[k] == ' ' || buf[k] == '\0')
                {
                    buf[k] = '\0';
                    new_argv[j++] = &buf[start];
                    start = k + 1;
                }
            }
            new_argv[j] = 0; // Gán NULL

            int pid = fork();
            if(pid == 0){
                // Tiến trình con: chạy lệnh
                exec(new_argv[0], new_argv);
                exit(0);
            } else {
                // Tiến trình cha: chờ con xong
                wait(0);
            }
            n = 0;

        }
        else buf[n++] = c;
    }

    
    exit(0);
}