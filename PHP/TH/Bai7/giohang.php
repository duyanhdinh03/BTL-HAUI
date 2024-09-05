<?php
$code = $GET['code']; // vi lay qua parameter trog duong dan the a - tai dong 51
$name = $_GET['name'];
$price = $_GET['price'];

//kiem tra xem co gio hang chua
session_start();

$cart = [];
if(isset($_SESSION['cart'])){
    $cart = $_SESSION['cart'];
}

//them hang 
//kiem tra xem hang da co trog gio chua
if(array_key_exists($code, $cart)){
    $cart[$code]['quantity']++;
}
// else{
//     $cart[$code] = ['name' => $name, 'quantity'=>1, 'price'=>$price];
// }

// cap nhat lai session
$_SESSION['cart'] = $cart;

//redirect den hien thi gio hang
header("Location:listcart.php");

?>