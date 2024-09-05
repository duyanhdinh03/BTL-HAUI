<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bai 3</title>
    <style>
        form{
            color: white;
            font-size: 20px;
            width: 600px; 
            margin: auto; 
            padding: 10px;
            background-color: lightseagreen;
        }
        div{
            margin: 10px 20px;
        }
    </style>
</head>
<body>
    <form action="" method="POST" >
        <div>
            <label>Mời nhập số nguyên: </label>
            <input type="text" name="number" id="number"
                <?php 
                    if(isset($_POST['btnTim'])){
                        echo "value =". $_POST['number']."";
                    }
                ?> 
            />
        </div>
        
        <div>
            <label>Số nguyên tố: </label>
            <input type="text" value="
                <?php 
                function isPrimeNumber($n){   
                        
                    for ($i = 2; $i <= sqrt($n); $i++) {
                        if ($n % $i == 0) {
                            return false;
                        }
                    }
                    return $n >= 2; 
                }
                if(isset($_POST['btnTim'])){
                    $num = $_POST['number'];
                    
                    for($i = 2; $i < $num; $i ++) {
                        if (isPrimeNumber ( $i )) {
                            echo ($i . " ");
                        }
                    }
                    
                }
                ?> "
            />
        </div>
        <div>
            <button type="submit" name="btnTim">Tìm</button>
            <button type="reset" name="btnXoa">Bỏ qua</button>
        </div>
    </form>
</body>
</html>