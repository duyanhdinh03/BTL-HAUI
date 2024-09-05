<?php
    $code = $_POST["id"];
    $name = $_POST["name"];
    $price = $_POST["price"];

    session_start();
    if (isset($_SESSION['cart']))
        $cart = $_SESSION['cart'];
    else
        $cart = [];

    if (array_key_exists($code, $cart)){
        $cart[$code]["quantity"]++;
    }
    else {
            $cart[$code] = ["name" => $name, "quantity" => 1, "price"=>$price];
    }
    $_SESSION['cart'] = $cart;
    header("Location:GioHang2.php");
?>
