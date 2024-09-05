<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Danh sách hàng</title>
</head>
<body>
    <a href="formhang.php"><button type="button">Thêm</button></a>
    <table width="100%" border="1" style="border-collapse: collapse;">
        <tr>
            <th>STT</th>
            <th>Mã hàng</th>
            <th>Tên hàng</th>
            <th>Nhóm hàng</th>
            <th>Số lượng</th>
            <th>Đơn giá</th>
            <th>Ảnh</th>
            <th>Action</th>
        </tr>

        <?php
        $fh = fopen("hang.txt", "r") or die("Can't open file!");

        $count = 0;
        while(!feof($fh)){
            $count ++;

            $line = fgets($fh);
            if($line != ""){
                $temp = explode("\t", $line);
            ?>
                <tr>
                    <td><?=$count?></td>
                    <td><?=$temp[0]?></td>
                    <td><?=$temp[1]?></td>
                    <td><?=$temp[2]?></td>
                    <td><?=$temp[3]?></td>
                    <td><?=$temp[4]?></td>
                    <td>
                        <img src="<?=$temp[5]?>" width="200px"/>
                    </td>
                    <td>
                        <a href="giohang.php?code=<?=$temp[0]?>&name=<?=$temp[1]?>&category=<?=$temp[2]?>&quantity=<?=$temp[3]?>&price=<?=$temp[4]?>"><button>Mua ngay</button></a>
                    </td>

                </tr>
                <?php
                
                }
            }
        ?>
        </tr>
    </table>

</body>
</html>