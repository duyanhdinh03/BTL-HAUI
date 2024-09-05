<?php
$name = $_POST['name'];
$desc = $_POST['desc'];

// ket noi csdl
include("connect.php");

// thực hiện truy vấn
$sql = "INSERT INTO tblcategory(Name, Description) VALUES ('$name', '$desc') ";

mysqli_query($conn, $sql); // truyen vao csld, cau lenh

//dong ket noi
mysqli_close($conn);
header("Location:listCate.php");
?>