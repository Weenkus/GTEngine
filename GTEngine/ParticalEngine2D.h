#pragma once

#include <vector> 

namespace GTEngine {

	class ParticalBatch2D;
	class SpriteBatch;

	class ParticalEngine2D
	{
	public:
		ParticalEngine2D();
		~ParticalEngine2D();

		// After adding partical batches to the partical engine,
		// the engine is reponsible for batch deallocation.
		void addParticalBatch(ParticalBatch2D* particalBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticalBatch2D*> m_batches;
	};

}

