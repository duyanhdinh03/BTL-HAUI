<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
        <table width="100%" border=1 style="border-collapse: collapse">
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
                if (isset($_SESSION['cart']))
                    $cart = $_SESSION['cart'];
                else
                    $cart = [];
                foreach($cart as $code=>$value){
                    $total += $value["quantity"]*$value["price"];
                    ?>
                    <tr>
                        <td><?=$code?></td>
                        <td><?=$value["name"]?></td>
                        <td><?=$value["quantity"]?></td>
                        <td><?=$value["price"]?></td>
                        <td><?=$value["quantity"]*$value["price"]?></td>
                    </tr>
                    <?php
                }
            ?>
            <tr>
                <td colspan = "4" style="text-align:right">Tổng tiền</td>
                <td><?=$total?></td>
            </tr>
        </table>
        <a href="./GianHang.php"><button>Quay lại</button></a>
        <a href="./order.php?total=<?=$total?>"><button>Đặt hàng</button></a>
</body>
</html>