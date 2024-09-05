<?php 
$code = $_POST["code"];
$name = $_POST["name"];
$grName = $_POST["grName"];
$quantity = $_POST["quantity"];
$price = $_POST["price"];
$fn = $_FILES['image']['name'];
$image = "images/".basename($fn);

include("connection.php");
$sqli = "SELECT * FROM tblcategory WHERE Name = $grName";
$result = mysqli_query($conn, $sqli);
$row = mysqli_fetch_assoc($result);

$sql = "INSERT INTO tblitems (code, name, cateId, quantity, price, imgpath) 
        VALUES('$code', '$name', '$row[Cid]', '$quantity', '$price', '$image') ";
mysqli_query($conn, $sql);

mysqli_close($conn);
header("Location: listProduct.php");

?>