<?php
$code = $_POST['code'];
$name = $_POST['name'];
$desc = $_POST['desc'];
if(file_exists("category.txt")){
    $fh = fopen("category.txt", "a");
}
else{
    $fh = fopen("category.txt", "w");
}
fwrite($fh, $code."\t". $name ."\t". $desc. "\n");
fclose($fh);
header("Location: formhang.php");
?>