<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Danh sách đơn hàng</title>
</head>
<body>
    <table width="80%" border="1" style="border-collapse: collapse; margin:auto;">
        <tr>
            <th>STT</th>
            <th>Mã đơn hàng</th>
            <th>Thời gian đặt hàng</th>
            <th>Tổng tiền</th>
            <th>Hành động</th>
        </tr>
    <?php 
        // mở file orders.txt để đọc, mỗi đơn hàng là 1 dòng của hàng
        if(file_exists("orders.txt")){
            $fh = fopen("orders.txt", "r");
            $count = 0;
            $total = 0;
            while(!feof($fh)){
                //doc 1 dong
                $line = fgets($fh);
                if(!empty($line)){
                    $count++;
                    $temp = explode("\t", $line);
                    $total += $temp[2];
                    ?>
                    <tr>
                        <td><?=$count?></td>
                        <td><?=$temp[0]?></td>
                        <td><?=$temp[1]?></td>
                        <td><?=$temp[2]?></td>
                        <td><a href="listOrderDetails.php?id=<?=$temp[0]?>"><button>Xem chi tiết hóa đơn</button></a></td>
                    </tr>

                    <?php 
                }

            }
        }

    ?>
    <tr>
        <td colspan="3">Tổng tiền</td>
        <td><?=$total?></td>
    </tr>
    </table>
</body>
</html>