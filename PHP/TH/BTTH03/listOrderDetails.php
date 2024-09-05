<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hiển thị chi tiết đơn hàng</title>
    <style>
        .center{
            align-items: center;
        }
    </style>
</head>
<body>
    <table border="1" width="50%" style="border-collapse: collapse; margin:auto;">
        <tr>
            <th class="center">STT</th>
            <th>Mã hàng</th>
            <th>Tên hàng</th>
            <th class="center">Số lượng</th>
            <th>Đơn giá</th>
            <th>Tổng tiền</th>
        </tr>
        <?php 
        // lấy mã đơn hàng
        $id = $_GET['id'];
        if(file_exists("orderdetails.txt")){
            $fh = fopen("orderdetails.txt", "r");
            $count = 0;
            while(!feof($fh)){
                //doc 1 dong
                $line = fgets($fh);
                if(!empty($line)){
                    $temp = explode("\t", $line);
                    if($temp[0] == $id){
                        $total = $temp[4]*$temp[3];
                        $count++;
                        ?>
                        <tr>
                            <td class="center"><?=$count?></td>
                            <td><?=$temp[1]?></td>
                            <td class="center"><?=$temp[2]?></td>
                            <td><?=$temp[3]?></td>
                            <td><?=$temp[4]?></td>
                            <td><?=$total?></td>
                        </tr>
                    <?php 
                    }
                }

            }
        }
        ?>
    </table>
</body>
</html>