#pragma warning(disable: 4793)

#include "Mouse.h"

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace FrameworkTest
{
	[TestClass]
	public ref class MouseTest
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
		void IsLeftButtonDown()
		{
			IsButtonDown(&Mouse::isLeftButtonDown, MouseState(true), MouseState());
		};


		[TestMethod]
		void IsRightButtonDown()
		{
			IsButtonDown(&Mouse::isRightButtonDown, MouseState(false, true), MouseState());
		};


		void IsButtonDown(bool (*isButtonDown)(), const MouseState& buttonDown, const MouseState& buttonUp)
		{
			Assert::AreEqual(false, isButtonDown());

			Mouse::setState(buttonDown);

			Assert::AreEqual(false, isButtonDown());

			for (int i = 0; i < 3; ++i)
			{
				Mouse::update();
				Assert::AreEqual(true, isButtonDown());
			}

			Mouse::setState(buttonUp);

			Assert::AreEqual(true, isButtonDown());

			for (int i = 0; i < 3; ++i)
			{
				Mouse::update();
				Assert::AreEqual(false, isButtonDown());
			}
		}


		[TestMethod]
		void GetPosition()
		{
			Mouse::setVisible(true);

			Point beginPosition = Point(10, 100);
			Point endPosition = Point(20, 200);

			MouseState begin(false, false, 0, beginPosition);
			MouseState end(false, false, 0, endPosition);

			AreEqual(Point(), (Mouse::getPosition)());

			Mouse::setState(begin);

			AreEqual(Point(), (Mouse::getPosition)());

			for (int i = 0; i < 5; ++i)
			{
				Mouse::update();
				AreEqual(beginPosition, (Mouse::getPosition)());
			}

			Mouse::setState(end);

			AreEqual(beginPosition, (Mouse::getPosition)());

			for (int i = 0; i < 5; ++i)
			{
				Mouse::update();
				AreEqual(endPosition, (Mouse::getPosition)());
			}
		}


		void AreEqual(const Point& p0, const Point& p1)
		{
			Assert::AreEqual(p0.x, p1.x);
			Assert::AreEqual(p0.y, p1.y);
		}


		[TestMethod]
		void GetMouseWheelDelta()
		{
			Assert::AreEqual(0, Mouse::getMouseWheelDelta());

			Mouse::setState(MouseState(false, false, 100));

			Assert::AreEqual(0, Mouse::getMouseWheelDelta());

			Mouse::update();
			Assert::AreEqual(100, Mouse::getMouseWheelDelta());

			for (int i = 0; i < 5; ++i)
			{
				Mouse::update();
				Assert::AreEqual(0, Mouse::getMouseWheelDelta());
			}

			Mouse::setState(MouseState(false, false, 100));
			Mouse::setState(MouseState(false, false, 200));
			Mouse::setState(MouseState(false, false, -50));
			Mouse::update();

			Assert::AreEqual(250, Mouse::getMouseWheelDelta());
		};
	};
}
