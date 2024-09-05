<?php 
$url = "localhost";
$username = "root";
$password = "12345678";
$dbname = "banhang";

$conn = mysqli_connect($url, $username, $password, $dbname);

if(!$conn){
    die("Kết nối không thành công".mysqli_connect_error());
}

?>