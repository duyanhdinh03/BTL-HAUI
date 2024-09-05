<?php 

$id = $_POST['id'];
$name = $_POST["name"];
$grName = $_POST["grName"];
$quantity = $_POST["quantity"];
$price = $_POST["price"];
$fn = $_FILES['image']['name'];
$image = "images/".basename($fn);

include("connection.php");
$sqli = "UPDATE listproducts SET 
         name = '$name', grName = '$grName', quantity = '$quantity', price =  '$price', imagepath = '$image') WHERE id = '$id'";
mysqli_query($conn, $sqli);

mysqli_close($conn);
header("Location: listProduct.php");

?>