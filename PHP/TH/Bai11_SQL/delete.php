<?php
$id = $_POST['id'];

include("configuration.php");
$conn = mysqli_connect($url, $username, $password, $dbname);
if(!$conn){ // ket noi that bai
    die("Kết nối không thành công ".mysqli_connect_error());
}
$sql = "DELETE FROM tblcategory WHERE id = $id";
mysqli_query($conn, $sql);
mysqli_close($conn);
header("Location:listcategory.php");
?>