// #include "typedefs.h"


// using namespace std;

// static void *send_data(vector<uint32_t> data) {
// 	snd_ctx *ctx = (snd_ctx*) context;
// 	ctx->sock->Send(ctx->snd_buf, ctx->snd_bytes);
// 	return 0;
// }

// //Apparently OT+Hashing don't use this function
// static void snd_and_rcv(uint8_t* snd_buf, uint32_t snd_bytes, uint8_t* rcv_buf, uint32_t rcv_bytes, CSocket* sock) {
// 	pthread_t snd_task;
// 	bool created, joined;
// 	snd_ctx ctx;
// 	//Start new sender thread
// 	ctx.sock = sock;
// 	ctx.snd_buf = snd_buf;
// 	ctx.snd_bytes = snd_bytes;
// 	created = !pthread_create(&snd_task, NULL, send_data, (void*) &(ctx));

// 	//receive
// 	sock->Receive(rcv_buf, rcv_bytes);
// 	assert(created);

// 	joined = !pthread_join(snd_task, NULL);
// 	assert(joined);
// }