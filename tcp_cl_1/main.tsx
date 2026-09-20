import React, { useState } from 'react';
import { render, Box, Text } from 'ink';
import TextInput from 'ink-text-input';
import koffi from "koffi";
import 'dotenv/config'
import libTcp from "./src/libTcp.js"

const start_str = `Welcome
Chat app Example

`;

const ITEM_DATA = [
  {id: "-1" , type: "info", title: start_str},
];

const TEST_TEXT_DATA = `Test-Text-data-001 , Test-Text-data-002 , Test-Text-data-003 , Test-Text-data-004 , Test-Text-data-005, Test-Text-data-006
Test-Text-data-001 , Test-Text-data-002 , Test-Text-data-003 , Test-Text-data-004 , Test-Text-data-005, Test-Text-data-006
Test-Text-data-001 , Test-Text-data-002 , Test-Text-data-003 , Test-Text-data-004 , Test-Text-data-005, Test-Text-data-006
Test-Text-data-001 , Test-Text-data-002 , Test-Text-data-003 , Test-Text-data-004 , Test-Text-data-005, Test-Text-data-006
`;
function SearchCommandLine() {
  const [query, setQuery] = useState('');
  const [submittedQuery, setSubmittedQuery] = useState('');
  const [items, setItems] = useState(ITEM_DATA);

  const handleSubmit = async (value) => {
    setSubmittedQuery(value);
    setQuery("")
    try{
      let targetStr = value;
      let responseText = "";
      const lib = koffi.load('./sample.dll');
      const send_tcp = lib.func('char* send_tcp(const char* input)');
      let resp = libTcp.getJsonData(targetStr);
      let bl_update = targetStr.startsWith("update:");
      let bl_select = targetStr.startsWith("select:");
      if(bl_update === true || bl_select === true){
        responseText = send_tcp(resp);
        if(bl_select){
          responseText = libTcp.getSelectText(responseText)
        }
      }      

      const target = items;
      let uuid = crypto.randomUUID();
      target.push({id: uuid , type: "user" , title: targetStr})
      uuid = crypto.randomUUID();
      target.push({id: uuid , type: "ai" , title: responseText})

      setItems(target)
      setSubmittedQuery("");
    }catch(e){
      console.log(e)
    }
    //setTimeout(async() => {
    //}, 1000);		
  };


  return (
  <Box flexDirection="column" padding={1}>
    <Box flexDirection="row">
      <Box width="80%" flexDirection="column" padding={0}>
        {items.map(item => {
          if(item.type === "user"){
            return(
              <Box key={item.id} flexDirection="column" borderStyle="round" marginBottom={0} padding={1}>
                <Text bold color="cyan">You:</Text>
                <Text>{item.title}</Text>
              </Box>
            )
          }      
          if(item.type === "ai"){
            return(
              <Box key={item.id} flexDirection="column" borderStyle="round" marginBottom={0} padding={1}>
                <Text bold color="cyan">AI:</Text>
                <Text>{item.title}</Text>
              </Box>
            )
          }      
          if(item.type === "info"){
            return(
              <Box key={item.id} height={10} flexDirection="column" borderStyle="round" padding={1}>
                <Text bold color="cyan">{item.title}</Text>
                <Text>End: Ctrl + c</Text>
              </Box>
            )
          }      
        })}
      </Box>
      <Box width="20%" flexDirection="column" padding={1}>
        <Text bold >AppName:</Text>
        <Text bold marginTop={1}>version: 0.9.1</Text>
      </Box>    
    </Box>  
    {submittedQuery ? (
    <Box marginTop={1} marginBottom={1} marginLeft={1}>
      <Text color="green" marginTop={1}>Please Wait...</Text>
    </Box>
    ):(<Box />)}
    <Box flexDirection="column" borderStyle="round" padding={1}>
      <Box marginRight={1}>
        <Text bold color="cyan">Input: </Text>
        <TextInput 
          value={query} 
          onChange={setQuery} 
          onSubmit={handleSubmit} 
        />
      </Box>
      <Box marginTop={0} flexDirection="column">
        <Text dimColor>Type your text and press Enter:</Text>
      </Box>
    </Box>

  </Box>
  );  
}

const main = async function(){
  render(<SearchCommandLine />);
}
main();
