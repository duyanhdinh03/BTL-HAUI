
<?php
$id = $_POST['id'];
$name = $_POST['name'];
$desc = $_POST['desc'];

include("configuration.php");
$conn = mysqli_connect($url, $username, $password, $dbname);
if(!$conn){ // ket noi that bai
    die("Kết nối không thành công ".mysqli_connect_error());
}
$sql = "UPDATE tblcategory SET name = $name, desc = $desc WHERE id = $id";
$result = mysqli_query($conn, $sql);
$row = mysqli_fetch_assoc($result);
mysqli_close($conn);
header("Location:listcategory.php");
?>

