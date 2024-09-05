<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" 
    rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" 
    crossorigin="anonymous">
    <title>Document</title>
</head>
<body>
    <a href="admin.php"><button type="button" class="btn btn-primary">Homepage</button></a>
    <a href="inputUser.html"><button type="button" class="btn btn-primary">Create new</button></a>
    <table class="table table-bordered" width="70%" border="1" style="border-collapse: collapse;">
        <tr>
            <th>ID</th>
            <th>Username</th>
            <th>Password</th>
            <th>Role</th>
            <th></th>
        </tr>
        <?php 
            include("connect.php");

            $sql = "SELECT * FROM tbluser";
            $result = mysqli_query($conn, $sql);
            $count = 0;

            while($row = mysqli_fetch_assoc($result)){
                $count++;
            ?>
            <tr>
                <td><?=$count?></td>
                <td><?=$row['uname']?></td>
                <td><?=$row['upass']?></td>
                <td><?=$row['urole']?></td>
                <td>
                    <a href="edituser.php?id=<?=$row['id']?>"><button type="button"  class="btn btn-primary">Edit</button></a>
                    <button type="button" onclick="XacNhanXoa(<?=$row['id']?>)"  class="btn btn-primary">Delete</button>
                </td>
            </tr>
            <?php
            } // end while 
            mysqli_close($conn);
        ?>
    </table>
    <form action="deleteuser.php" method="post" id="formXoa">
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
