<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Giỏ hàng</title>
</head>
<body>
    <table border="1" width="100%">
        <tr>
            <th>Mã hàng</th>
            <th>Tên hàng</th>
            <th>Số lượng</th>
            <th>Đơn giá</th>
            <th>Thành tiền</th>
        </tr>
        <?php 
        $total = 0;
        session_start();
        if(isset($_SESSION['cart'])){
            $cart = $_SESSION['cart'];
            
            foreach($cart as $code=>$value){
                $total += (int)$value['quantity'] * (float)$value['price'];
                ?>
                <tr>
                    <td><?=$code?></td>
                    <td><?=$value['name']?></td>
                    <td><?=$value['quantity']?></td>
                    <td><?=$value['price']?></td>
                    <td><?=(int)$value['quantity'] * (float)$value['price']?></td>
                </tr>
                <?php
            }
            
        }
        ?>
        <tr>
            <td colspan="4">Tổng tiền</td>
            <td><?=$total?></td>
        </tr>
    </table>
    <a href="listProduct.php"><button>Mua tiếp</button></a>
    <a href="order.php"><button>Đặt hàng</button></a>
</body>
</html>