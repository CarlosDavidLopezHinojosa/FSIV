### For KNN

```bash
./train_clf --clf=0 -f=2 --f_params=100 --knn_K=5 --s_ratio=0.4 ../data/pollen_train/train ../data/pollen_valid/valid ../data/pollen_test/test ../models/KNN/knn3bovw.yml
```

Best results: knn5_bovw.yml

```bash
Using a K-NN classifier with k=5

Training ... done.
Computing training accuracy ... done.
Training accuracy: 0.627187

Validating ... done.
Validation accuracy: 0.466667

Saving the model to '../models/KNN/knn3bovw.yml'.
Model size: 9.53132 Mb.
Size score max(0.0, 1.0-(model_size_mb/dataset_size_mb)) = 0.947119
Predicted final score 2*(acc*size_score)/(acc+size_score) = 0.625256
```
