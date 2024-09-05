<?php 

include("configuration.php");


$name = $_POST['name'];
$grName = $_POST['grName'];
$quantity = $_POST['quantity'];
$price = $_POST['price'];

$directory = "imges/";
$fn = basename($_FILES['image']['name']);
$path = $directory.$fn;

$sql = "INSERT INTO tblproduct (name, grName, quantity, price, image ) VALUES ('$name', '$grName', '$quantity', '$price', '$path')";
mysqli_query($conn, $sql);

mysqli_close($conn);
header("Location:listProd.php");

?>