

<?php

// ket noi csdl
include("configuration.php");
$conn = mysqli_connect($url, $username, $password, $dbname);

if(!$conn){ // ket noi that bai
    die("Kết nối không thành công ".mysqli_connect_error());
}

?>
