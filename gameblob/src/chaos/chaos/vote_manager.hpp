#ifndef CHAOS_VOTE_BLOB_HPP
#define CHAOS_VOTE_BLOB_HPP

class ChaosVote;

class ChaosVoteManager {
	ChaosVote* pVote;

	void createRandomVote();

   public:
	ChaosVoteManager();

	~ChaosVoteManager();

	/// Called on update.
	void update();

	void render();
};

/// Initialize the vote system.
void chaosVoteInit();

/// Shutdown the vote system.
void chaosVoteShutdown();

/// Get a pointer to the singleton instance of the
/// vote manager.
ChaosVoteManager* chaosVoteMan();

#endif
