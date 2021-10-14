Chương trình demo code Elgamal

1. Công cụ & môi trường 
+ GNU (9.4)
+ Visual Studio Code (1.6)
+ Extension: C/C++ 
+ In setting: Run code with Terminal

2. Cách chạy chương trình
+ Mở từng tệp tin của mỗi thuật toán
+ Run Code (Ctrl + Alt + N)
+ Khi bắt đầu chạy chương trình, nhấn 1 sẽ tạo khóa công khai xuất ra file key/public_key.txt
( trong đó khóa công khai lưu giá trị của 3 tham số q, g,h), khóa bí mật xuất ra file 
key/private_key.txt( trong đó khóa bí mật lưu giá trị của 3 tham số q, g,x).
+ Nhấn chọn 2 sẽ lấy dữ liệu khóa công khai từ file public_key.txt và lấy bản rõ từ file key/plaintext.txt 
để mã hóa, và xuất bản mã ra 2 file là msg/c1.txt và msg/c2.txt, nằm ở thư mục msg.
+ Nhấn chọn 3 sẽ lấy dữ liệu khóa bí mật từ file key/private_key.txt và 2 bản mã từ file msg/c1.txt và msg/c2.txt
để tiến hành giải mã ra bản rõ ban đầu.
