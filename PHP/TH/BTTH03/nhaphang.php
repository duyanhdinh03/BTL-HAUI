<?php
$code = $_POST['code'];
$name = $_POST['name'];
$category = $_POST['category'];
$directory = "images/";
$quantity = $_POST['quantity'];
$price = $_POST['price'];
$fn = basename($_FILES['image']['name']);
$path = $directory.$fn;
if(file_exists("hang.txt")){
    $fh = fopen("hang.txt", "a");
}
else{
    $fh = fopen("hang.txt", "w");
}
fwrite($fh, $code. "\t". $name ."\t". $category. "\t" . $quantity. "\t". $price. "\t" . $path. "\n");
fclose($fh);
if(!file_exists($path)){
    move_uploaded_file($_FILES['image']['tmp_name'], $path);
}
header("Location: danhsachhang.php");
?>