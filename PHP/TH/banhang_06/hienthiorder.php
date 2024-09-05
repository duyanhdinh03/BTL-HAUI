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
                <th>Thời gian</th>
                <th>Tổng tiền</th>
                <th>Thao tác</th>
            </tr>
            <?php
                include "./connect.php";
                $sql = "SELECT * FROM tblorder";
                $result = mysqli_query($conn, $sql);
                if (mysqli_num_rows($result) > 0){
                    while($row = mysqli_fetch_assoc($result)){
                        ?>
                        <tr>
                            <td><?=$row["ID"]?></td>
                            <td><?=$row["DATE"]?></td>
                            <td><?=$row["TOTAL"]?></td>
                            <td><a href="./chitietorder.php?code=<?=$row["ID"]?>"><button>Chi tiết</button></a></td>
                        </tr>
                        <?php
                    }
                }
            ?>
        </table>
</body>
</html>