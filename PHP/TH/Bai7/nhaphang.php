<?php 

//đọc dl từ form
$code = $_POST['code'];
$name = $_POST['name'];
$quantity = $_POST['quantity'];
$price = $_POST['price'];
$category = $_POST['category'];

// doc thong tin cua file
$directory = "/images";
$fn = basename($_FILES['image']['name']);
//
$path = $directory.$fn;

if(file_exists("hang.txt")){
    $fh = fopen("hang.txt", "a");
}
else{
    $fh = fopen("hang.txt", "w");
}

fwrite($fh, $code."\t".$name."\t".$category."\t".$quantity."\t".$price."\t".$path."\n");
fclose($fh);

// kiem tra anh ton tai chua
if(file_exists($path)){
    // di chuyen tu thu muc temp vao thu muc tho duong dan
    move_uploaded_file($_FILES['image']['temp_name'], $path);
}

//redirect đến trag dánhachhang
header("Location:danhsachhang.php");
?>