<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Danh sach hang</title>
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
            <th>Hành động</th>
        </tr>
        <?php 

            // Mở file để đọc
            $fh = fopen("hang.txt", "r") or die("Đọc file bị lỗi");

            //đọc lần lượt từng dòng
            $count = 0;
            while(!feof($fh)){
                $count++;

                //đọc 1 dòng
                $line = fgets($fh);
                if($line != ""){
                    $temp= explode("\t", $line);
                    
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
                        <a href="giohang.php?code=<?=$temp[0]?>&name =<?=$temp[1]?>&price=<?=$temp[4]?>">
                        <button type="button">Mua ngay</button>
                        </a>
                    </td>
                </tr>
                    <?php 
                }
            }
        ?>
    </table>
</body>
</html>