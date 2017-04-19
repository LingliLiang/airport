#pragma once

#include <string>

#define ACTION_NEXT_ITEM              14
#define ACTION_PREV_ITEM              15
#define ACTION_VOLUME_UP            88
#define ACTION_VOLUME_DOWN          89
#define ACTION_MUTE                 91

/*!
  \ingroup actionkeys
  \brief class encapsulating information regarding a particular user action to be sent to windows and controls
  */
class CAction
{
public:
  CAction(int actionID, const std::string &name);

  CAction(const CAction& other) { *this = other; }
  CAction& CAction::operator=(const CAction& rhs)
  {
	  if (this != &rhs)
	  {
		  m_id = rhs.m_id;
		  m_name = rhs.m_name;
		  m_text = rhs.m_text;
	  }
	  return *this;
  }


  /*! \brief Identifier of the action
   \return id of the action
   */
  int GetID() const { return m_id; };

  /*! \brief Human-readable name of the action
   \return name of the action
   */
  const std::string &GetName() const { return m_name; };
  
  /*! \brief Text of the action if any
   \return text payload of this action.
   */
  const std::string &GetText() const { return m_text; };
  
  /*! \brief Set the text payload of the action
   \param text to be set
   */
  void SetText(const std::string &text) { m_text = text; };

private:
  int          m_id;
  std::string   m_name;
  std::string  m_text;
};
