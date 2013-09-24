#include "Keyboard.h"

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace FrameworkTest
{
	[TestClass]
	public ref class KeyboardTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		property TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes

		[ClassInitialize()]
		static void MyClassInitialize(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ testContext) 
		{
		};
		
		[ClassCleanup()]
		static void MyClassCleanup() 
		{
		};
		
		[TestInitialize()]
		void MyTestInitialize()
		{
		};
		
		[TestCleanup()]
		void MyTestCleanup() 
		{
		};
		
		#pragma endregion 

		[TestMethod]
		void KeyboardStateTest()
		{
			Test(false, false, false);

			for (int i = 0; i < 5; ++i)
			{
				Keyboard::setKey(0, true);
				Test(false, false, false);
			}

			Keyboard::update();
			Test(true, true, false);

			for (int i = 0; i < 5; ++i)
			{
				Keyboard::update();
				Test(true, false, false);
			}

			for (int i = 0; i < 5; ++i)
			{
				Keyboard::setKey(0, false);
				Test(true, false, false);
			}

			Keyboard::update();
			Test(false, false, true);

			for (int i = 0; i < 5; ++i)
			{
				Keyboard::update();
				Test(false, false, false);
			}
		};


		void Test(bool isKeyDown, bool isKeyPress, bool isKeyRelease)
		{
			Assert::AreEqual(isKeyDown, Keyboard::isKeyDown(0));
			Assert::AreEqual(isKeyPress, Keyboard::isKeyPress(0));
			Assert::AreEqual(isKeyRelease, Keyboard::isKeyRelease(0));
		};
	};
}
