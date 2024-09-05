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
                <th>Mã đơn hàng</th>
                <th>Mã hàng</th>
                <th>Số lượng</th>
                <th>Đơn giá</th>
            </tr>
            <?php
                $code = $_GET["code"];
                include "./connect.php";
                $sql = "SELECT * FROM tblorderdeltails WHERE MA_DH = $code";
                $result = mysqli_query($conn, $sql);
                if (mysqli_num_rows($result) > 0){
                    while($row = mysqli_fetch_assoc($result)){
                        ?>
                        <tr>
                            <td><?=$row["MA_DH"]?></td>
                            <td><?=$row["MA_HANG"]?></td>
                            <td><?=$row["SO_LUONG"]?></td>
                            <td><?=$row["DON_GIA"]?></td>
                        </tr>
                        <?php
                    }
                }
            ?>
        </table>
</body>
</html>