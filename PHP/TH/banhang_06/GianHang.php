<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        * {
            font-size: 30px;
        }
        .right {
            text-align: center;
        }
        .center {
            text-align: center;
        }
    </style>
</head>
<body>

        <a href="./GioHang2.php"><button type="button">Giỏ Hàng</button></a>
        <table width="100%" border=1 style="border-collapse: collapse">
            <tr>
                <th>Mã hàng</th>
                <th>Tên hàng</th>
                <th>Số lượng</th>
                <th>Đơn giá</th>
                <th>Hình ảnh</th>
                <th>Chọn mua</th>
            </tr>
                <?php
                    include "./connect.php";
                    $sql = "SELECT * FROM tblproduct";
                    $result = mysqli_query($conn, $sql);
                    if (mysqli_num_rows($result) > 0){
                        while($row = mysqli_fetch_assoc($result)){
                    ?>
                        <tr>
                            <td><?=$row["ID"]?></td>
                            <td><?=$row["NAME"]?></td>
                            <td><?=$row["SO_LUONG"]?></td>
                            <td><?=$row["DON_GIA"]?></td>
                            <td><img src="<?=$row["IMAGE"]?>" alt=""></td>
                            <td>
                                <form action="./GioHang.php" method="post">
                                    <input type="hidden" name="id" value="<?=$row["ID"]?>">
                                    <input type="hidden" name="name" value="<?=$row["NAME"]?>">
                                    <input type="hidden" name="price" value="<?=$row["DON_GIA"]?>">
                                    <button type="submit">Chọn mua</button>
                                </form>
                            </td>
                        </tr>
                    <?php
                        }
                    }
                ?>
        </table>
</body>
</html>