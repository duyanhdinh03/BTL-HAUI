<?php 
// 1 tạo ra 1 đơn hàng --> tạo 1 dòng trog file donhang.txt
if(file_exists("sequence.txt")){
    $fh = fopen("sequence.txt", "r");
    $no = intval(fgets($fh));
}
else{
    $no = 0;
}
fclose($fh);
$no++;
// cập nhật lại stt vào file seuence.txt
$fh = fopen("sequence.txt", "w");
fwrite($fh, $no);
fclose($fh);

//mở file don hang để ghi một dòng mới
if(file_exists("orders.txt")){
    $fh = fopen("orders.txt", "a");
}
else{
    $fh = fopen("orders.txt", "w");
}
$ct = date("d/m/Y H:i:s");
// tính tổng tiền
$total = $_GET['total'];
//ghi đơn hàng gồm số hóa đơn no, timestamp ct, tổng tiền
fwrite($fh, $no."\t".$ct."\t".$total."\n");
fclose($fh);

//2 Tạo nhiều đơn hàng
if(file_exists("orderdetails.txt")){
    $fh = fopen("orderdetails.txt", "a");
}else{
    $fh = fopen("orderdetails.txt", "w");
}

//ghi nhiều dòng vào file
session_start();
if(isset($_SESSION['cart'])){
    $cart = $_SESSION['cart'];
    foreach($cart as $code => $value){
        fwrite($fh, $no."\t".$code."\t".$value['name']."\t".$value['quantity']."\t".$value['price']."\n");
    }
}
fclose(($fh));
unset($_SESSION['cart']);
header("Location: listorder.php");
?>