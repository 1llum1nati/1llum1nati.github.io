<?php
$email = $_POST['email'];
$email = htmlspecialchars($email);
$email = urldecode($email);
$email = trim($email);
//echo $email;
if (mail("appdevice@yandex.ru", "Заявка с сайта", "E-mail:".$email. ,"From \r\n"))
 {     echo "сообщение успешно отправлено"; 
} else { 
    echo "при отправке сообщения возникли ошибки";
}?>
