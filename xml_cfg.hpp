#ifndef __XML_CONFIG_INCLUDE__
#define __XML_CONFIG_INCLUDE__

#include  "tinyxml.h"
#include <string>
#include <map>


typedef std::map<std::string,std::string> AttrMap ; 

class ConfigManager
{
public:
	ConfigManager(void);
	~ConfigManager(void);

public:
	bool IsOpen() const { return m_pDoc!=NULL ;} 
	bool LoadConfigFile( std::string filename) ; 

    bool GetNodePointerByNameFromRoot(std::string strNodeName,TiXmlElement* &Node) ;  
    bool GetNodePointerByName(TiXmlElement* pStartEle,std::string &strNodeName,TiXmlElement* &Node) ;  

	
	bool QueryNode_Attribute( TiXmlElement *pNode ,AttrMap &AttMap)  ; 
	bool QueryNode_Attribute( TiXmlElement *pNode ,std::string strNodeName , AttrMap &AttMap)  ; 
	bool QueryNode_Text(TiXmlElement *pNode ,std::string &strText) ;
    bool QueryNode_Text(TiXmlElement *pNode ,std::string strNodeName , std::string &strText) ;


	
     bool ModifyNode_Text(TiXmlElement *pNode ,std::string strText )  ;  
	 bool ModifyNode_Text(TiXmlElement *pNode ,std::string strNodeName ,std::string strText)  ;

	 bool ModifyNode_Attribute(TiXmlElement *pNode , AttrMap &AttMap) ;  
     bool ModifyNode_Attribute(TiXmlElement *pNode ,std::string strNodeName ,AttrMap &AttMap)   ;

	
	 void Save();

public:


static double Str2Double(std::string value);
static bool Str2Bool(std::string value);
static int Str2Int(std::string value);

private:

	TiXmlDocument *m_pDoc  ; 
	std::string m_XMLFile ; 

};


std::string get_node_text(ConfigManager& xml_cfg , 
	std::string pnode_name, std::string node_name)  ; 


#endif 
