<?php 

$url = "localhost";
$username = "root";
$password = "";
$dbname = "btth06";

$conn = mysqli_connect($url, $username, $password, $dbname);

if(!$conn){
    die("Kết nối không thành công".mysqli_connect_error());
}

?>