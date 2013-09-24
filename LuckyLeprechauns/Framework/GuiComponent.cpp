#include "GuiComponent.h"
#include "GuiManager.h"

GuiComponent::GuiComponent(GuiManager* guiManager) : BasicComponent(guiManager)
{
}

GuiComponent::~GuiComponent(void)
{
}

void GuiComponent::draw(const GameTime& gameTime)
{
}

void GuiComponent::onLostDevice()
{
	if(document)
	{
		getManager().getContext().UnloadDocument(document);
		document->RemoveReference();
	}
}

void GuiComponent::onResetDevice(const GraphicsDevice& device)
{
	loadDocument();
}

std::string GuiComponent::getElementText(const std::string& elementId) const
{
	RocketString text;
	document->GetElementById(convert(elementId))->GetInnerRML(text);
	return convert(text);
}

std::string GuiComponent::getElementProperty(const std::string& elementId, const std::string& propertyName) const
{
	RocketString propertyValue;
	document->GetElementById(convert(elementId))->GetProperty(convert(propertyName))->value.GetInto(propertyValue);
	return convert(propertyValue);
}

void GuiComponent::setElementText(const std::string& elementId, const std::string& text)
{
	document->GetElementById(convert(elementId))->SetInnerRML(convert(text));
}

void GuiComponent::setElementProperty(const std::string& elementId, const std::string& propertyName, const std::string& propertyValue)
{
	document->GetElementById(convert(elementId))->SetProperty(convert(propertyName), convert(propertyValue));
}

void GuiComponent::setElementClass(const std::string& elementId, const std::string& classValue)
{
	document->GetElementById(convert(elementId))->SetClassNames(convert(classValue));
}

void GuiComponent::loadDocument(const std::string& documentPath)
{
	this->documentPath = convert(documentPath);
	loadDocument();
}

void GuiComponent::loadDocument()
{
	document = getManager().getContext().LoadDocument(this->documentPath);
	
	if(document)
		document->Show();
}

const GuiComponent::RocketString GuiComponent::convert(const std::string& value) const
{
	return RocketString(value.c_str());
}

const std::string GuiComponent::convert(RocketString value) const
{
	return std::string(value.CString());
}