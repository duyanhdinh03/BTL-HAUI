<?php 
$name = $_POST["name"];
$grName = $_POST["grName"];
$quantity = $_POST["quantity"];
$price = $_POST["price"];
$fn = $_FILES['image']['name'];
$image = "images/".basename($fn);

include("connection.php");
$sqli = "INSERT INTO listproducts( name, grName, quantity, price, imagepath) 
        VALUES('$name', '$grName', '$quantity', '$price', '$image') ";
mysqli_query($conn, $sqli);

mysqli_close($conn);
header("Location: listProduct.php");

?>