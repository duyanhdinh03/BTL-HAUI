<?php
//đọc dl từ form
$code = $_POST['code'];
$name = $_POST['name'];
$desc = $_POST['desc'];

//lưu vào file
//kiem tra file ton tai hay chua
if(file_exists("nhom.txt")){
    $fh = fopen("nhom.txt", "a");
}else{
    $fh = fopen("nhom.txt", "w");
}

//ghi dl vao file

fwrite($fh, $code."\t".$name."\t".$desc."\n");

//dong file
fclose($fh);

//Redirect den form nhap tiep
header("Location:formhang.php");

?>