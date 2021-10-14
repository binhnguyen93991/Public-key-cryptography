# Cài đặt môi trường cần thiết 
## Yêu cầu HDH: Ubuntu
- Cài đặt trên môi trường window với WSL, thực hiện câu lệnh:
    wsl --install -d Ubuntu

## Yêu cầu package: GCC
- Trên môi trường Ubuntu, thực hiện câu lệnh:
    sudo apt-get install gcc

# Các bước biên dịch chương trình:
- Bật cửa sổ terminal tại thư mục chính
- gõ lệnh:
    wsl
để vào môi trường Ubuntu

## Biên dịch 
- Thực hiện câu lệnh
### Server: 
    gcc src/server.c -lm -std=c99 -o test/server.elf

### Client: 
    gcc src/client.c -lm -std=c99 -o test/client.elf

- Chương trình sau khi biên dịch sẽ được lưu tại thư mục Test

## Chạy chương trình :
- Ta cần bật 2 cửa sổ terminal và cả 2 cửa sổ đều truy cập vào wsl
### Tại cửa số thứ nhất, ta thực hiện câu lệnh:
    ./test/server.elf 127.0.0.1

### Tại cửa sổ thứ hai, ta thực hiện câu lệnh:
    ./test/client.elf 127.0.0.1 msg/input.txt

Các khóa trao đổi sẽ được hiển thị trên màn hình của server cũng như client
Thông điệp từ tệp input.txt sẽ được gửi từ bên client đến phía server.
Phía server nhận được thông điệp, in ra màn hình và giải mã thông điệp rồi ghi vào tệp output.txt

*** Hình ảnh hướng dẫn thực hiện được lưu trong thư mục /docs
