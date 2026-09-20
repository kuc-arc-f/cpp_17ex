
const libTcp = {
  
  /**
  *
  * @param
  *
  * @return
  */
  getJsonData: function(target){
    let retStr = "";
    let bl_update = target.startsWith("update:");
    let bl_select = target.startsWith("select:");
    //console.log("bl_update=", bl_update);
    //console.log("bl_select=", bl_select);
    let resp = "";
    if (bl_update) {
      const arr = target.split(":");
      if(arr && arr.length >= 2){
        //console.log("len=" , arr.length)
        //console.log(arr)
        const sendData = {
          action_name: "update",
          table: "",
          sql: arr[1]
        }
        const j1 = JSON.stringify(sendData)
        //console.log(j1)
        retStr = j1;
        return retStr;
      }
    }
    if (bl_select) {
      const arr = target.split(":");
      if(arr && arr.length === 2){
        //console.log("len=" , arr.length)
        //console.log(arr)
        const sendData = {
          action_name: "select",
          table: arr[1],
          sql: "SELECT * FROM " + arr[1] + ";"
        }
        const j1 = JSON.stringify(sendData)
        //console.log(j1)
        retStr = j1;
        return retStr;
      }
      if(arr && arr.length === 3){
        //console.log("len=" , arr.length)
        //console.log(arr)
        const sendData = {
          action_name: "select",
          table: arr[1],
          sql: arr[2]
        }
        const j1 = JSON.stringify(sendData)
        //console.log(j1)
        retStr = j1;
        return retStr;
      }
      
    }
  },

  /**
  *
  * @param
  *
  * @return
  */
  getSelectText: function(input){
      const o1 = JSON.parse(input)
      let resp_head_str = "";
      o1.columns.forEach((element) => {
        resp_head_str += "    " + element
      });
      let resp_row_str = "";
      o1.data.forEach((element) => {
        o1.columns.forEach((col_item) => {
          let col_data = element[col_item];
          resp_row_str += "    " + col_data
        });
        resp_row_str += "\n";
      });      

      let responseText = resp_head_str + "\n";
      responseText += resp_row_str;
      return responseText;
  },

}
export default libTcp;
