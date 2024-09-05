<?php
    $url = "localhost";
    $uname = "root";
    $upass = "";
    $dbname = "banhang";
    $conn = mysqli_connect($url,$uname,$upass,$dbname);
    if(!$conn){//thất bại
        die("Kết nối thất bại".mysqli_connect_error());
    }
?>