const voidRML = require("./voidrml.js");

const app = new voidRML.App(1280, 900);
let elem;

app.on("click", (target, e) => {
    console.log("click event", target.id, e);
    //if(target.id == elem.id) 
    {
        elem.setProperty("left", e.mouse_x + "px");
        elem.setProperty("top", e.mouse_y + "px");
        elem.setProperty("position", "absolute");
        console.log("setting pos");
    }
});

app.on("init", () => {
    app.loadFontFace("arial.ttf", true);
    console.log("creating doc");
    const doc = app.loadDocument("main.rml");
    console.log("doc visible", doc.isVisible());
    doc.setVisible(true);
    console.log("doc visible", doc.isVisible());


    elem = doc.createTextNode("test");
    doc.appendChild(elem);
    //elem.setProperty("position", "absolute");

    elem.id = "clickelem";
    console.log("elem id", elem.id);

    const attributes = elem.getAttributes();
    console.log("elem attributes", attributes);

    console.log("doc elements", doc.querySelectorAll("img")[0]);

    app.emit("someevent");
});

console.log("running", app.isRunning());
app.start();