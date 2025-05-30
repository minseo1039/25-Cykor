<?php
session_start();
$conn = mysqli_connect("localhost", "root", "1234", "cykor_db");
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Cykor Week 3</title>
</head>
<body>
    <h1>List Page</h1>
    <h2>Welcome to Song Min Seo's Cykor Week 3</h2>
    <ul>
        <li><a href="./index.php">Main</a></li>
        <?php
        if (isset($_SESSION['username'])) {
            echo "<li><a href='./logout.php'>Logout</a></li>";
        }
        else {
            echo "<li><a href='./login.php'>Login</a></li>";
            echo "<li><a href='./register.php'>Register</a></li>";
        }
        ?>
        <li><a href = "./list.php">List</a></li>
    </ul>

    <?php
    if (isset($_SESSION['username'])) {
        echo "<p>Creat the your context!</p>";
        echo "<a href='write.php'>Write</a>";
    }
    ?>

    <h3>List</h3>
    <?php
    $sql = "SELECT title, writer FROM posts";
    $result = mysqli_query($conn, $sql);

    if (mysqli_num_rows($result) > 0) {
        echo "<ul>";
        while ($row = mysqli_fetch_assoc($result)) {
            echo "<li><strong><a href='view.php?title=" . urlencode($row['title']) . "'>" . htmlspecialchars($row['title']) . "</a></strong> by " . htmlspecialchars($row['writer']) . "</li>";
        }
        echo "</ul>";
    } else {
        echo "<p>No posts yet.</p>";
    }

    mysqli_close($conn);
    ?>
</body>
</html>