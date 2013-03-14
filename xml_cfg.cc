#include "xml_cfg.hpp"
#include <stdlib.h>
#include <cassert>

ConfigManager::ConfigManager(void)
:m_pDoc(NULL)
{
}

ConfigManager::~ConfigManager(void)
{
}

bool ConfigManager::LoadConfigFile( std::string filename)

{
	if(filename.size() == 0 ) return false ; 

	m_pDoc = new TiXmlDocument();  
	assert(m_pDoc!=NULL && "m_pDoc==NULL") ;
	if (NULL==m_pDoc)  
	{  
		return false;  
	}  
	m_XMLFile = filename ; 
return 	m_pDoc->LoadFile(filename.c_str());  
}



bool ConfigManager::GetNodePointerByName(TiXmlElement* pStartEle,std::string &strNodeName,TiXmlElement* &Node)
{
	  
	if (strNodeName==pStartEle->Value())  
	{  
		Node = pStartEle;  
		return true;  
	}  
	TiXmlElement* pEle = pStartEle;    
	for (pEle = pStartEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())    
	{    
		
		if(GetNodePointerByName(pEle,strNodeName,Node))  
			return true;  
	}    
	return false;  

}

bool ConfigManager::GetNodePointerByNameFromRoot(std::string strNodeName,TiXmlElement* &Node) 
 {
	 TiXmlElement *pRootEle = m_pDoc->RootElement();  
	 if (NULL==pRootEle)  
	 {  
		 return false;  
	 }  
	return GetNodePointerByName(pRootEle ,strNodeName ,Node ) ; 


 }

bool ConfigManager::QueryNode_Attribute( TiXmlElement *pNode ,std::string strNodeName , AttrMap &AttMap) 
{
	
	 TiXmlElement *pEle = NULL ; 
	bool bFind =  GetNodePointerByName(pNode , strNodeName , pEle) ; 
	if(!bFind) return false ; 

return QueryNode_Attribute(pEle , AttMap) ; 
}

bool ConfigManager::QueryNode_Attribute( TiXmlElement *pNode ,AttrMap &AttMap) 
{
	assert(pNode!=NULL && "QueryNode_Attribute - pNode==NULL") ; 
	if (NULL!=pNode)  
	{  
		AttMap.clear() ; 
		TiXmlAttribute* pAttr = NULL;   
		for (pAttr = pNode->FirstAttribute(); pAttr; pAttr = pAttr->Next())    
		{    
			std::string strAttName = pAttr->Name();  
			std::string strAttValue = pAttr->Value();  
			AttMap.insert(std::make_pair<std::string , std::string>(strAttName,strAttValue));  
		}    
		return true;  
	}  
	else  
	{  
		return false;  
	}  
	return true;  

}


bool ConfigManager::QueryNode_Text(TiXmlElement *pNode ,std::string &strText) 
{
assert(pNode!=NULL && "QueryNode_Text - pNode==NULL") ;
	if (NULL!=pNode)  
	{  
		strText = pNode->GetText();   
		return true;  
	}  
	else  
	{  
		return false;  
	}  

}

 bool ConfigManager::QueryNode_Text(TiXmlElement *pNode ,std::string strNodeName , std::string &strText)
 {
	 TiXmlElement *pEle = NULL ; 
	 bool bFind =  GetNodePointerByName(pNode , strNodeName , pEle) ; 
	 assert(pEle!=NULL && "QueryNode_Text - GetNodePointerByName return NULL");
	 if(!bFind) return false ; 
	 return QueryNode_Text(pEle ,strText) ; 
 
 }

 
 bool ConfigManager::ModifyNode_Text(TiXmlElement *pNode ,std::string strText) 
 {
 
	 assert(pNode!=NULL && "ModifyNode_Text - pNod=NULL") ; 
	 if (NULL!=pNode)  
	 {  
		 pNode->Clear();    
		 TiXmlText *pValue = new TiXmlText(strText.c_str());  
		 pNode->LinkEndChild(pValue);  
		 return true;  
	 }  
	 else  
	 {
		 return false;  
	 }

 }


 bool ConfigManager::ModifyNode_Text(TiXmlElement *pNode ,std::string strNodeName ,std::string strText) 
 {
	 TiXmlElement *pEle = NULL ; 
	 bool bFind =  GetNodePointerByName(pNode , strNodeName , pEle) ; 
	 assert(pEle!=NULL && "ModifyNode_Text - GetNodePointerByName return NULL");
	 if(!bFind) return false ; 
	 return ModifyNode_Text(pEle ,strText) ; 

 }

 void ConfigManager::Save()
  {
	  assert(m_XMLFile.size()>0 && "m_XMLFile is empty!") ; 
  m_pDoc->SaveFile(m_XMLFile.c_str()) ; 
  }

 bool ConfigManager::ModifyNode_Attribute(TiXmlElement *pNode , AttrMap &AttMap) 
 {
	 assert(pNode!=NULL && "ModifyNode_Attribute - pNode==NULL!") ; 
	 if (NULL!=pNode)  
	 {  
		 TiXmlAttribute* pAttr = NULL;   
		 std::string strAttName = ("");  
		 std::string strAttValue = ("");  
		 for (pAttr = pNode->FirstAttribute(); pAttr; pAttr = pAttr->Next())    
		 {    
			 strAttName = pAttr->Name();  
			 AttrMap::iterator iter;  
			 for (iter=AttMap.begin();iter!=AttMap.end();iter++)  
			 {  
				 if (strAttName==iter->first)  
				 {  
					 pAttr->SetValue(iter->second.c_str());  
				 }  
			 }  
		 }    
		 return true;  
	 }  
	 else  
	 {  
		 return false;  
	 }  

 }

 bool ConfigManager::ModifyNode_Attribute(TiXmlElement *pNode ,std::string strNodeName ,AttrMap &AttMap)  
{
	TiXmlElement *pEle = NULL ; 
	bool bFind =  GetNodePointerByName(pNode , strNodeName , pEle) ; 
	assert(pEle!=NULL && "ModifyNode_Attribute - GetNodePointerByName return NULL");
	if(!bFind) return false ; 
	return ModifyNode_Attribute(pEle , AttMap) ; 
 }


 double ConfigManager::Str2Double(std::string value)
 {
 return atof(value.c_str()) ; 
 }


  

  bool ConfigManager::Str2Bool(std::string value)
  {
	  return (strcmp(value.c_str(),"true") == 0)  ; 
  }

  int ConfigManager::Str2Int(std::string value)
  {
  return atoi(value.c_str()) ;
  }

/////////////////////////////////////////////////
std::string get_node_text(ConfigManager& xml_cfg , 
	std::string pnode_name, std::string node_name) 
{


TiXmlElement* config_node = NULL ; 
std::string value_str ; 

if(!xml_cfg.GetNodePointerByNameFromRoot(pnode_name ,config_node ))
	return std::string() ; 

if(config_node==NULL)
	return std::string() ; 


if(!xml_cfg.QueryNode_Text(config_node , node_name	 , value_str))
	return std::string() ; 

return value_str ; 
}