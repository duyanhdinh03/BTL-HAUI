<?php 

include("connection.php");
$total = $_GET['total'];
$date = date("Y-m-d H:i:s");

$sql = "INSERT INTO tblorder(time, total) VALUES('$date', '$total')";
$result = mysqli_query($conn, $sql);

$sql2 = "SELECT * FROM tblorder";
$result2 = mysqli_query($conn, $sql2);

while($row = mysqli_fetch_assoc($result2)){
    $Oid = $row['id'];
    session_start();
    if(isset($_SESSION['cart'])){
        $cart = $_SESSION['cart'];

        foreach($cart as $id => $value){
            $quantity = $value['quantity'];
            $price = $value['price'];

            $sql3 = "INSERT INTO tablorderdetail(Oid, Pid, quantity, price) VALUES ('$Oid', '$code', '$quantity', '$price')";
            $result3 = mysqli_query($conn, $sql3);

            
        }
    }
}

unset($_SESSION['cart']);
mysqli_close($conn);
header("Location: ")

?>