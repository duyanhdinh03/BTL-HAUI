<?php 
$name = $_POST['name'];
$month = $_POST['month'];
$year = $_POST['year'];
$old = $_POST['old'];
$new = $_POST['new'];

$num = $new - $old;
if($num <=10){
$money = $num * 5000;
}
else if($num <=20){
    $money = 10 * 5000 + ($num-10) * 10000;
}
else if($num <=30){
    $money = 10 * 5000 + 10 * 10000 + ($num-20) * 15000;
}
else{
    $money = 10 * 5000 + 10 * 10000 + 10 * 15000 + ($num -30) * 20000;
}

$vat = $money * 0.1;
$bvmt = $money * 0.15;
$total = $money + $vat + $bvmt;

if(file_exists("water.txt")){
    $fh = fopen("water.txt", "a");
}
else{
    $fh = fopen("water.txt", "w");
}
fwrite($fh, $name."\t".$month."\t".$year."\t".$num."\t".$money."\t".$vat."\t".$bvmt."\t".$total."\n");
fclose($fh);
header("Location: danhsach.php");
?>