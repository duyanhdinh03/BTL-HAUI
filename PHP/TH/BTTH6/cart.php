<?
include("connection.php");
$code = $_GET['id'];
$name = $_GET['name'];
$price = $_GET['price'];

session_start();

$cart = [];

if(isset($_SESSION['cart'])){
    $cart = $_SESSION['cart'];
}

if(array_key_exists($code, $cart)){
    $cart[$code]['quantity']++;
}
else{
    $cart[$code] = ['name' => $name, 'quantity' =>1, 'price' => $price];
}

$_SESSION['cart']  = $cart;
header("Location: listcart.php");

?>