<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hiển thị giỏ hàng</title>
</head>
<body>

    <table width="100%" border="1" style="border-collapse: collapse;">
        <tr>
            <th>STT</th>
            <th>Mã hàng</th>
            <th>Tên hàng</th>
            <th>Số lượng</th>
            <th>Đơn giá</th>
            <th>Thành tiền</th>
        </tr>

        <?php

        // doc gio hang
            session_start();
            if(isset($_SESSION['cart'])){
                $cart = $_SESSION['cart'];
                $count = 0;
                $total= 0;
                foreach($cart as $code => $value){ // key:code & giatri thuoc mang value
                    $count++;
                    ?>
                    <tr>
                        <td><?=$count?></td>
                        <td><?=$code?></td>
                        <td><?=$value['name']?></td>
                        <td><?=$value['quantity']?></td>
                        <td><?=$value['price']?></td>
                        <td><?=$value['quantity']?> * <?=$value['price']?></td>
                        
                    </tr>
                    <?php 
                    
                    $total += $value['quantity'] * $value['price'];
                }
            }
        ?>
        <tr>
            <td>Tổng tiền</td>
            <td><?=$total?></td>
        </tr>

    </table>
    <a href="danhsachhang.php"><button type="button">Mua tiếp</button></a>
</body>
</html>