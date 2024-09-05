<?php
$name = $_POST['name'];
$desc = $_POST['desc'];

// ket noi csdl
include("configuration.php");
$conn = mysqli_connect($url, $username, $password, $dbname);

if(!$conn){ // ket noi that bai
    die("Kết nối không thành công ".mysqli_connect_error());
}
// thực hiện truy vấn
$sql = "INSERT INTO tblcategory(name, desc) VALUES ('$name', '$desc') ";

mysqli_query($conn, $sql); // truyen vao csld, cau lenh

//dong ket noi
mysqli_close($conn);
header("Location:listcategory.php");
?>