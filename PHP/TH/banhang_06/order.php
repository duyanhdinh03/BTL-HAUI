<?php
    $total = $_GET["total"];
    $date = date("Y/m/d");
    session_start();

    if ($_SESSION['cart'] != []) {
        $sql = "INSERT INTO `tblorder`(`DATE`, `TOTAL`) VALUES ('$date','$total')";
        include "./connect.php";
        mysqli_query($conn, $sql);
        $order_id = "SELECT * FROM tblorder ORDER BY ID DESC LIMIT 1";
        $result = mysqli_query($conn, $order_id);
        $id = mysqli_fetch_assoc($result)["ID"];
        var_dump($id);
        $cart = $_SESSION['cart'];
        foreach($cart as $code=>$value){
            $quantity = $value["quantity"];
            $price = $value["price"];
            $sql2 = "INSERT INTO `tblorderdeltails`(`MA_DH`, `MA_HANG`, `SO_LUONG`, `DON_GIA`) VALUES ('$id','$code','$quantity','$price')";
            mysqli_query($conn, $sql2);
        }
        $_SESSION['cart'] = [];
        mysqli_close($conn);
    }
    else
        echo "Đơn hàng rỗng";

    
?>