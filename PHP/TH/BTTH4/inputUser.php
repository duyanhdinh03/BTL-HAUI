<?php
$name = $_POST['uname'];
$pass = $_POST['upass'];
$role = $_POST['urole'];

// ket noi csdl
include("connect.php");

// thực hiện truy vấn
$sql = "INSERT INTO tbluser(uname, upass, urole) VALUES ('$name', '$pass', '$role') ";

mysqli_query($conn, $sql); // truyen vao csld, cau lenh

//dong ket noi
mysqli_close($conn);
header("Location:listuser.php");
?>