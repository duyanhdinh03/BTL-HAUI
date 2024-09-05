<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <a href="category.html"><button type="button">Thêm</button></a>
    <table width="70%" border="1" style="border-collapse: collapse;">
        <tr>
            <th>STT</th>
            <th>Tên nhóm</th>
            <th>Mô tả</th>
            <th>Hành động</th>
        </tr>
        <?php 
            include("configuration.php");
            $conn = mysqli_connect($url, $username, $password, $dbname);

            if(!$conn){ // ket noi that bai
                die("Kết nối không thành công ".mysqli_connect_error());
            }

            $sql = "SELECT * FORM tblcategory ";
            $result = mysqli_query($conn, $sql);
            $count = 0;

            while($row = mysqli_fetch_assoc($result)){
                $count++;
            ?>
            <tr>
                <td><?=$count?></td>
                <td><?=$row['name']?></td>
                <td><?=$row['desc']?></td>
                <td>
                    <a href="edtcategory.php?id=<?=$row['id']?>"><button type="button">Sửa</button></a>
                    <button type="button" onclick="XacNhanXoa(<?=$row['id']?>)">Xóa</button>
                </td>
            </tr>
            <?php
            } // end while 
            mysqli_close($conn);
        ?>
    </table>
    <form action="deletecategory.php" method="post" id="formXoa">
            <input type="hidden" name="id" id="id"/>

    </form>
    <script type="text/javascript">
        function XacNhanXoa(id){
            var tl = confirm("Bạn có thật sự muốn xóa không");
            if(tl){
                document.getElementById("id").value = id;
                document.getElementById("formXoa").submit();
            }
            alert(id);
        }
    </script>
</body>
</html>