<?php 

$id = $_POST['id'];
$code = $_POST['code'];
$name = $_POST["name"];
$grName = $_POST["grName"];
$quantity = $_POST["quantity"];
$price = $_POST["price"];
$fn = $_FILES['image']['name'];
$image = "images/".basename($fn);

include("connection.php");
$sqli = "UPDATE tblitems SET name = '$name', grName = '$grName', quantity = '$quantity', 
        price =  '$price', imgpath = '$image' WHERE id = '$id'";
mysqli_query($conn, $sqli);

mysqli_close($conn);
header("Location: listProduct.php");

?>