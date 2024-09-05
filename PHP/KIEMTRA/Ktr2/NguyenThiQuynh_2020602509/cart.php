<?
$id = $_GET['id'];
$code = $_GET['code'];
$name = $_GET['name'];
$price = $_GET['price'];

session_start();

$cart = [];

if(isset($_SESSION['cart'])){
    $cart = $_SESSION['cart'];
}

if(array_key_exists($id, $cart)){
    $cart[$id]['quantity']++;
}
else{
    $cart[$id] = ['code' => $code, 'name' => $name, 'quantity' =>1, 'price' => $price];
}

$_SESSION['cart']  = $cart;
header("Location: listcart.php");

?>