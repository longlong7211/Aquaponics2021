firebase.initializeApp({
    apiKey: 'AIzaSyCl5KFiev2ak1o4f1i2-vsUSpmsnPolJ-A',
    authDomain: 'landingpageaqua.firebaseapp.com',
    projectId: 'landingpageaqua'
});

var db = firebase.firestore();
function SetSttBtn(elid, el, val1, val2) {
    document.getElementById(el).classList.remove("bg-secondary");
    document.getElementById(el).classList.remove(val1);
    document.getElementById(el).classList.add(val2);
    if (val2 == "bg-success") {
        document.getElementById(elid).innerHTML = "ON";
    } else {
        document.getElementById(elid).innerHTML = "OFF";
    }
}
//get lần đầu tiên
db.collection("device").get().then((querySnapshot) => {
    querySnapshot.forEach((doc) => {
        // console.log(`${doc.id} => ${doc.data().nhietdo}`);
        if (doc.id == "data") {
            document.getElementById("nhietdo").innerHTML = doc.data().nhietdo;
            document.getElementById("ph").innerHTML = doc.data().ph;
            document.getElementById("tds").innerHTML = doc.data().tds;
            document.getElementById("thucan").innerHTML = doc.data().thucan;
        } else {
            //nút 1
            if (doc.data().btn1 == "0") {
                SetSttBtn("ttDen1", "bgDen1", "bg-success", "bg-danger");
            } else {
                SetSttBtn("ttDen1", "bgDen1", "bg-danger", "bg-success");
            }
            // nút 2
            if (doc.data().btn2 == "0") {
                SetSttBtn("ttDen2", "bgDen2", "bg-success", "bg-danger");
            } else {
                SetSttBtn("ttDen2", "bgDen2", "bg-danger", "bg-success");
            }
            //nút 3
            if (doc.data().btn3 == "0") {
                SetSttBtn("ttBom", "bgBom", "bg-success", "bg-danger");
            } else {
                SetSttBtn("ttBom", "bgBom", "bg-danger", "bg-success");
            }
            //nút 3 có 3 trạng thái

        }
    });
});
//bắt sự kiện của data
db.collection("device").doc("data")
    .onSnapshot((doc) => {
        document.getElementById("nhietdo").innerHTML = doc.data().nhietdo;
        document.getElementById("ph").innerHTML = doc.data().ph;
        document.getElementById("tds").innerHTML = doc.data().tds;
        document.getElementById("thucan").innerHTML = doc.data().thucan;
    });

db.collection("device").doc("controll")
    .onSnapshot((doc) => {
         //nút 1
         if (doc.data().btn1 == "0") {
            SetSttBtn("ttDen1", "bgDen1", "bg-success", "bg-danger");
        } else {
            SetSttBtn("ttDen1", "bgDen1", "bg-danger", "bg-success");
        }
        // nút 2
        if (doc.data().btn2 == "0") {
            SetSttBtn("ttDen2", "bgDen2", "bg-success", "bg-danger");
        } else {
            SetSttBtn("ttDen2", "bgDen2", "bg-danger", "bg-success");
        }
        //nút 3
        if (doc.data().btn3 == "0") {
            SetSttBtn("ttBom", "bgBom", "bg-success", "bg-danger");
        } else {
            SetSttBtn("ttBom", "bgBom", "bg-danger", "bg-success");
        }
        //nút 3 có 3 trạng thái
    });
