<?php
    $url = "localhost";
    $uname = "root";
    $upass = "12345678";
    $dbname = "BanHang";
    $conn = mysqli_connect($url, $uname, $upass, $dbname);
    if(!$conn){
        die("Kết nối thất bại".mysqli_connect_error());
    }
?>