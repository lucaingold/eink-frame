const posts = [];
const images = [
  'pic/new/1.bmp',
  'pic/new/2.bmp',
  'pic/new/4.bmp',
  'pic/new/5.bmp',
  'pic/new/6.bmp',
  'pic/new/7.bmp',
  'pic/new/8.bmp',
  'pic/new/9.bmp',
  'pic/new/10.bmp',
  'pic/new/11.bmp',
  'pic/new/12.bmp',
  'pic/new/13.bmp',
  'pic/new/14.bmp',
  'pic/new/15.bmp',
  'pic/new/16.bmp',
  'pic/new/17.bmp',
  'pic/new/18.bmp',
  'pic/new/19.bmp',
  'pic/new/20.bmp',
  'pic/new/21.bmp',
  'pic/new/22.bmp',
  'pic/new/23.bmp',
  'pic/new/24.bmp',
  'pic/new/25.bmp',
  'pic/new/26.bmp',
  'pic/new/27.bmp',
  'pic/new/28.bmp',
  'pic/new/29.bmp',
  'pic/new/30.bmp',
  'pic/new/31.bmp',
];

let imageIndex = 0;

for (let i = 1; i <= 30; i++) {
  let item = {
    id: i,
    title: `Image ${i}`,
    image: images[imageIndex].replace('.bmp', '_thumb.png'),
    path: `path=./pic/new/` + images[imageIndex].replace('pic/new/', ''),
  };
  posts.push(item);
  imageIndex++;
  if (imageIndex > images.length - 1) imageIndex = 0;
}
